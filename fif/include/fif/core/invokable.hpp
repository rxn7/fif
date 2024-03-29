#pragma once

#include <functional>
#include <mutex>
#include <type_traits>
#include <utility>
#include <vector>

namespace fif {
	template<typename... A> class Callback final {
	public:
		Callback(std::function<void(A...)> func) : m_Hash(func.target_type().hash_code()), m_Bound(std::move(func)) {}

		bool operator==(const Callback<A...> &cb) { return m_Hash == cb.m_Hash; }

		bool operator!=(const Callback<A...> &cb) { return m_Hash != cb.m_Hash; }

		constexpr size_t hash_code() const { return m_Hash; }

		Callback<A...> &invoke(A... args) {
			m_Bound(static_cast<A &&>(args)...);
			return (*this);
		}

		void operator()(A... args) { m_Bound(static_cast<A &&>(args)...); }

	protected:
		size_t m_Hash;
		std::function<void(A...)> m_Bound;
	};

	template<typename... A> class Invokable final {
	public:
		Invokable<A...> &hook(const Callback<A...> cb) {
			m_Callbacks.push_back(cb);
			return (*this);
		}

		Invokable<A...> &unhook(const Callback<A...> cb) {
			std::erase_if(m_Callbacks, [cb](Callback<A...> c) { return cb.hash_code() == c.hash_code(); });
			return (*this);
		}

		Invokable<A...> &rehook(const Callback<A...> cb) {
			m_Callbacks.clear();
			m_Callbacks.push_back(cb);
			return (*this);
		}

		Invokable<A...> &clear() {
			m_Callbacks.clear();
			return (*this);
		}

		Invokable<A...> &invoke(A... args) {
			for(Callback<A...> cb : m_Callbacks)
				cb.invoke(static_cast<A &&>(args)...);

			return (*this);
		}

	protected:
		std::vector<Callback<A...>> m_Callbacks;
	};
}// namespace fif
