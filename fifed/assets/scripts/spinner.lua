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
		self.transform.angleRadians = self.i
		local cosi = math.cos(self.i)
		local sini = math.sin(self.i)
		self.transform.position = Vec2(sini * 200, cosi * 200)
		self.sprite.tint.r = math.floor((cosi + 1) * 0.5 * 255)
		self.sprite.tint.g = 255 - self.sprite.tint.r
		self.circle.radius = cosi * 100
		self.i = self.i + dt * 10
	end,
}
