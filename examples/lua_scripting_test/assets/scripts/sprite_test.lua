return {
	sprite = nil,
	label = nil,

    -- Gets called when the script is attached in the editor
    init = function(self)
        self.sprite = self.entity:require_sprite_component()
        self.label = self.entity:require_label_component()
        self.label.text = "Sprite Test"
    end,

    -- Gets called when the game is started
    update = function(self, dt)
        local str = 'sprite.tint: ' .. tostring(self.sprite.tint) .. '\n'
        str = str .. 'sprite.size: ' .. tostring(self.sprite.size) .. '\n'
        str = str .. 'sprite.texture: ' .. tostring(self.sprite.texture) .. '\n'
        self.label.text = str
    end,
}
