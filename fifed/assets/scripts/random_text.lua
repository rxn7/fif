return {
    label = nil,
    timer = 0,
    interval = 0.2,

    init = function(self)
        self.label = self.entity:require_label_component()
    end,

    update = function(self, dt)
        self.timer = self.timer + dt
        if self.timer < self.interval then
            return
        end

        self.timer = 0
        self.label.text = ""

        for _=1,10 do
            local random_char = string.char(math.random(32, 126))
            self.label.text = self.label.text .. random_char
        end
    end,
}
