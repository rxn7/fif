return {
	t = nil,
	i = 0,

	init = function(self)
		self.t = self.entity:get_transform_component()
	end,

	update = function(self, dt)
		self.t.angleRadians = self.i
		self.t.position.x = math.sin(self.i) * 100
		self.t.position.y = math.cos(self.i) * 100
		self.i = self.i + dt * 10
	end,
}
