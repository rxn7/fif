#include "fif/core/ecs/scene_serializer.hpp"
#include "fif/core/ecs/components/tag_component.hpp"
#include "fif/core/ecs/components/transform_component.hpp"

// TODO: Check if GFX module is used
#include "fif/gfx/color.hpp"
#include "fif/gfx/components/circle_component.hpp"
#include "fif/gfx/components/quad_component.hpp"
#include "fif/gfx/components/sprite_component.hpp"
#include "fif/lua_scripting/components/lua_script_component.hpp"
#include "fif/lua_scripting/lua_scripting_module.hpp"

#include <fstream>
#include <sstream>

namespace YAML {
	void handle_exception(const char *what) {
		fif::core::Logger::error("YAML exception: %s", what);
		std::terminate();
	}

	Emitter &operator<<(Emitter &out, const fif::vec2 &v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	Emitter &operator<<(Emitter &out, const fif::vec3 &v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	Emitter &operator<<(Emitter &out, const fif::gfx::Color &v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << static_cast<i32>(v.r) << static_cast<i32>(v.g) << static_cast<i32>(v.b) << static_cast<i32>(v.a) << YAML::EndSeq;
		return out;
	}

	template<> struct convert<fif::vec2> {
		static Node encode(const fif::vec2 &v) {
			Node n;
			n.push_back(v.x);
			n.push_back(v.y);
			return n;
		}

		static bool decode(const Node &node, fif::vec2 &v) {
			if(!node.IsSequence() || node.size() != 2)
				return false;
			v.x = node[0].as<f32>();
			v.y = node[1].as<f32>();
			return true;
		}
	};

	template<> struct convert<fif::vec3> {
		static Node encode(const fif::vec3 &v) {
			Node n;
			n.push_back(v.x);
			n.push_back(v.y);
			n.push_back(v.z);
			return n;
		}

		static bool decode(const Node &node, fif::vec3 &v) {
			if(!node.IsSequence() || node.size() != 3)
				return false;
			v.x = node[0].as<f32>();
			v.y = node[1].as<f32>();
			v.z = node[2].as<f32>();
			return true;
		}
	};

	template<> struct convert<fif::gfx::Color> {
		static Node encode(const fif::gfx::Color &v) {
			Node n;
			n.push_back(v.r);
			n.push_back(v.g);
			n.push_back(v.b);
			n.push_back(v.a);
			return n;
		}

		static bool decode(const Node &node, fif::gfx::Color &v) {
			if(!node.IsSequence() || node.size() != 4)
				return false;
			v.r = node[0].as<u8>();
			v.g = node[1].as<u8>();
			v.b = node[2].as<u8>();
			v.a = node[3].as<u8>();
			return true;
		}
	};
}// namespace YAML

namespace fif::core {
	SceneSerializer::SceneSerializer(Scene &scene) : m_Scene(scene) {}

	static void serialize_entity(const Entity &entity, YAML::Emitter &yaml);
	static void deserialize_entity(const Entity &entity, YAML::Emitter &yaml);
	template<typename T> static void serialize_component(const Entity &entity, YAML::Emitter &yaml, std::function<void(T &t)> func);
	template<typename T> static void try_get_component_node(const YAML::Node &node, std::function<void(const YAML::Node &node)> func);

	void SceneSerializer::serialize(const std::string &path) {
		Logger::info("Serializing scene: %s", path.c_str());

		YAML::Emitter yaml;
		yaml << YAML::BeginMap;
		yaml << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene.for_each([&](EntityID id) { serialize_entity(Entity(m_Scene, id), yaml); });
		yaml << YAML::EndSeq;
		yaml << YAML::EndMap;

		std::ofstream fileStream(path);
		fileStream << yaml.c_str();
	}

	void SceneSerializer::deserialize(const std::string &path) {
		Logger::info("Deserializing scene: %s", path.c_str());

		std::ifstream fileStream(path);

		std::stringstream ss;
		ss << fileStream.rdbuf();

		const YAML::Node rootNode = YAML::Load(ss.str());

		const YAML::Node entitiesNode = rootNode["Entities"];
		if(!entitiesNode)
			return;

		for(const YAML::Node &entityNode : entitiesNode) {
			Entity entity(m_Scene, m_Scene.get_registry().create());

#define DESERIALIZE(type, funcBody) try_get_component_node<type>(entityNode, [&entity](const YAML::Node &compNode) funcBody);

			DESERIALIZE(TagComponent, { entity.add_component<TagComponent>(compNode["Tag"].as<std::string>().c_str()); });
			DESERIALIZE(TransformComponent, {
				auto &c = entity.add_component<TransformComponent>();
				c.position = compNode["Position"].as<vec2>();
				c.scale = compNode["Scale"].as<vec2>();
				c.angleRadians = compNode["AngleRadians"].as<f32>();
			});
			DESERIALIZE(gfx::SpriteComponent, {
				auto &c = entity.add_component<gfx::SpriteComponent>();
				c.size = compNode["Size"].as<vec2>();
				c.tint = compNode["Tint"].as<gfx::Color>();

				const std::string texturePath = compNode["TexturePath"].as<std::string>();
				if(!texturePath.empty())
					c.set_texture(std::make_shared<gfx::Texture>(texturePath));
			});
			DESERIALIZE(gfx::QuadComponent, {
				auto &c = entity.add_component<gfx::QuadComponent>();
				c.size = compNode["Size"].as<vec2>();
				c.tint = compNode["Tint"].as<gfx::Color>();
			});
			DESERIALIZE(gfx::CircleComponent, {
				auto &c = entity.add_component<gfx::CircleComponent>();
				c.tint = compNode["Tint"].as<gfx::Color>();
				c.radius = compNode["Radius"].as<f32>();
			});
			DESERIALIZE(lua_scripting::LuaScriptComponent, { lua_scripting::LuaScriptingModule::get_instance()->attach_script(entity, compNode["Path"].as<std::string>()); });
		}
	}

	template<typename T> static void try_get_component_node(const YAML::Node &entityNode, std::function<void(const YAML::Node &node)> func) {
		const YAML::Node componentNode = entityNode[std::string(entt::type_id<T>().name())];
		if(componentNode)
			func(componentNode);
	}

	static void serialize_entity(const Entity &entity, YAML::Emitter &yaml) {
		yaml << YAML::BeginMap;
		yaml << YAML::Key << "UUID" << YAML::Value << static_cast<u32>(entity.m_ID);// TODO: UUID

#define SERIALIZE(type, funcBody) serialize_component<type>(entity, yaml, [&yaml](auto &c) funcBody)

		SERIALIZE(TagComponent, { yaml << YAML::Key << "Tag" << YAML::Value << c.tag; });
		SERIALIZE(TransformComponent, {
			yaml << YAML::Key << "Position" << c.position;
			yaml << YAML::Key << "Scale" << c.scale;
			yaml << YAML::Key << "AngleRadians" << YAML::Value << c.angleRadians;
		});
		SERIALIZE(gfx::SpriteComponent, {
			yaml << YAML::Key << "Tint" << c.tint;
			yaml << YAML::Key << "Size" << c.size;
			yaml << YAML::Key << "TexturePath" << YAML::Value << (c.p_texture != nullptr ? c.p_texture->m_Path : "");// TODO: Asset UUID
		});
		SERIALIZE(gfx::QuadComponent, {
			yaml << YAML::Key << "Tint" << c.tint;
			yaml << YAML::Key << "Size" << c.size;
		});
		SERIALIZE(gfx::CircleComponent, {
			yaml << YAML::Key << "Tint" << c.tint;
			yaml << YAML::Key << "Radius" << c.radius;
		});
		SERIALIZE(lua_scripting::LuaScriptComponent, { yaml << YAML::Key << "Path" << YAML::Value << c.filepath; });

		yaml << YAML::EndMap;
	}

	template<typename T> static void serialize_component(const Entity &entity, YAML::Emitter &yaml, std::function<void(T &t)> func) {
		if(entity.has_component<T>()) {
			yaml << YAML::Key << std::string(entt::type_id<T>().name());
			yaml << YAML::BeginMap;
			func(entity.get_component<T>());
			yaml << YAML::EndMap;
		}
	}
}// namespace fif::core
