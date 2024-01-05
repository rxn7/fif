local function lerp(a, b, alpha)
    return a + (b - a) * alpha
end

return {
    transform = nil,
    move_speed = 300,

    init = function(self)
        self.transform = self.entity:require_transform_component()
    end,

    update = function(self, dt)
        local move = Vec2(0,0)

        if Input.is_key_held(Input.Key.A) then move.x = move.x - 1 end
        if Input.is_key_held(Input.Key.D) then move.x = move.x + 1 end
        if Input.is_key_held(Input.Key.W) then move.y = move.y + 1 end
        if Input.is_key_held(Input.Key.S) then move.y = move.y - 1 end

        move:normalize()

        self.transform.angleRadians = lerp(self.transform.angleRadians, move.x * math.pi * 0.25, dt * 50)
        self.transform.position = self.transform.position + move * self.move_speed * dt
    end,
}
