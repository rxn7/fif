return {
	tag = nil,
	label = nil,

    -- Gets called when the script is attached in the editor
    init = function(self)
        self.tag = self.entity:require_tag_component()
        self.label = self.entity:require_label_component()
        self.label.text = "Tag Test"
    end,

    -- Gets called when the game is started
    update = function(self, dt)
        local str = 'tag.tag: ' .. self.tag.tag
        self.label.text = str
    end,
}
