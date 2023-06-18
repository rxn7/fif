local timer = 0
local frames = 0

local function tick(self, dt)
	self.fps = frames
	self.dt = dt
	timer = 0
	frames = 0
end

return {
	fps = 0,
	dt = 0,

	init = function(self)
		Logger.info("fps.lua loaded id: " .. tostring(self.entity.id))
	end,

	update = function(self, dt)
		frames = frames + 1;
		timer = timer + dt

		if timer >= 1 then
			tick(self, dt)
		end
	end,
}
