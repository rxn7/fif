return {
	uuid = nil,
	label = nil,

    -- Gets called when the script is attached in the editor
    init = function(self)
        self.uuid = self.entity:require_uuid_component()
        self.label = self.entity:require_label_component()
        self.label.text = "UUID Test"
    end,

    -- Gets called every frame in the runtime / playtime
    update = function(self, dt)
        local str = 'uuid.uuid: ' .. tostring(self.uuid.uuid)
        self.label.text = str
    end,
}
