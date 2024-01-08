return {
    label = nil,

    init = function(self)
        self.label = self.entity:require_label_component()
    end,

    start = function(self)
        self:randomize()
    end,

    update = function(self, dt)
        if Input.is_key_just_pressed(Input.Key.Space) then 
            self:randomize()
        end
    end,

    randomize = function(self)
        self.label.text = ""
        for _=1,10 do
            local random_char = string.char(math.random(32, 126))
            self.label.text = self.label.text .. random_char
            self.label.color = Color.random()
        end

        Logger.info(self.label.text)
    end
}
