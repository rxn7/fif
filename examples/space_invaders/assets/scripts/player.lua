return {
	transform = nil,
	move_speed = 500,

	init = function(self)
		self.transform = self.entity:get_transform_component()
	end,

	update = function(self, dt)
		self.transform = self.entity:get_transform_component()

		local move = Vec2(0,0)

		if Input.is_key_held(Input.Key.D) or Input.is_key_held(Input.Key.Right) then
			move.x = move.x + 1
		end

		if Input.is_key_held(Input.Key.A) or Input.is_key_held(Input.Key.Left) then
			move.x = move.x - 1
		end

		if Input.is_key_held(Input.Key.W) or Input.is_key_held(Input.Key.Up) then
			move.y = move.y + 1
		end

		if Input.is_key_held(Input.Key.S) or Input.is_key_held(Input.Key.Down) then
			move.y = move.y - 1
		end

		self.transform.position = self.transform.position + move * dt * self.move_speed
	end
}
