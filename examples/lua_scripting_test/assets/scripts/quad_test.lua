return {
	quad = nil,
	label = nil,

    -- Gets called when the script is attached in the editor
    init = function(self)
        self.quad = self.entity:require_quad_component()
        self.label = self.entity:require_label_component()
        self.label.text = "Quad Test"
    end,

    -- Gets called when the game is started
    update = function(self, dt)
        local str = 'quad.tint: ' .. tostring(self.quad.tint) .. '\n'
        str = str .. 'quad.size: ' .. tostring(self.quad.size) .. '\n'
        self.label.text = str
    end,
}
