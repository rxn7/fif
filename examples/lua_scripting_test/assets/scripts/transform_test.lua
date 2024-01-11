return {
	transform = nil,
	label = nil,

    -- Gets called when the script is attached in the editor
    init = function(self)
        self.transform = self.entity:require_transform_component()
        self.label = self.entity:require_label_component()
        self.label.text = "Transform Test"
    end,

    -- Gets called when the game is started
    update = function(self, dt)
        local str = 'transform.position: ' .. tostring(self.transform.position) .. '\n'
        str = str .. 'transform.scale: ' .. tostring(self.transform.scale) .. '\n'
        str = str .. 'transform.angleRadians: ' .. tostring(self.transform.angleRadians)
        self.label.text = str
    end,
}
