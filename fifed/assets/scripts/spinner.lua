return {
	transform = nil,
	sprite = nil,
	circle = nil,
	i = 0,

	init = function(self)
		self.transform = self.entity:require_transform_component()
		self.sprite = self.entity:require_sprite_component()
		self.circle = self.entity:require_circle_component()
	end,

	update = function(self, dt)
		local cosi = math.cos(self.i)
		local sini = math.sin(self.i)
		self.transform.angleRadians = cosi * math.pi * 2.0
		self.transform.position = Vec2(sini * 200, cosi * 200)

		self.sprite.size.x = (cosi + 1) * 200 + 100

		self.sprite.tint.r = math.floor((cosi + 1) * 0.5 * 255)
		self.sprite.tint.g = 255 - self.sprite.tint.r
		self.sprite.tint.b = math.floor((sini + 1) * 0.5 * 255)

		self.circle.tint.r = 255 - self.sprite.tint.r
		self.circle.tint.g = 255 - self.sprite.tint.g
		self.circle.tint.b = 255 - self.sprite.tint.b

		self.circle.radius = cosi * 100
		self.i = self.i + dt
	end,
}
