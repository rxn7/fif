return {
	circle = nil,
	label = nil,

    -- Gets called when the script is attached in the editor
    init = function(self)
        self.circle = self.entity:require_circle_component()
        self.label = self.entity:require_label_component()
        self.label.text = "Circle Test"
    end,

    -- Gets called when the game is started
    update = function(self, dt)
        local str = 'circle.tint: ' .. tostring(self.circle.tint) .. '\n'
        str = str .. 'circle.radius: ' .. tostring(self.circle.radius) .. '\n'
        self.label.text = str
    end,
}
