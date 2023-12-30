return {
    label = nil,

    init = function(self)
        self.label = self.entity:require_label_component()
    end,

    update = function(self, dt)
        self.label.text = ""

        for i=1,10 do
            local random_char = string.char(math.random(97, 97 + 25))
            self.label.text = self.label.text .. random_char
        end
    end,
}
