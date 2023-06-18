return {
	transform = nil,
	sprite = nil,
	i = 0,

	init = function(self)
		self.transform = self.entity:get_transform_component()
		if self.transform == nil then Logger.error("TransformComponent is missing") end

		self.sprite = self.entity:get_sprite_component()
		if self.sprite == nil then Logger.error("SpriteComponent is missing") end
	end,

	update = function(self, dt)
		self.transform.angleRadians = self.i
		local cosi = math.cos(self.i)
		local sini = math.sin(self.i)
		self.transform.position.x = sini * 200
		self.transform.position.y = cosi * 200
		self.sprite.tint.r = math.floor((cosi + 1) * 0.5 * 255)
		self.sprite.tint.g = math.floor((sini + 1) * 0.5 * 255)
		self.i = self.i + dt * 10
	end,
}
