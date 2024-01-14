return {
	transform = nil,
	move_speed = 500,

	init = function(self)
		self.transform = self.entity:get_transform_component()
	end,

	update = function(self, dt)
		local move = 0

		if Input.is_key_held(Input.Key.D) or Input.is_key_held(Input.Key.Right) then
			move = move + 1
		end

		if Input.is_key_held(Input.Key.A) or Input.is_key_held(Input.Key.Left) then
			move = move - 1
		end

		self.transform.position.x = self.transform.position.x + move * dt * self.move_speed
	end
}
