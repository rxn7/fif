return {
    -- Gets called when the script is attached in the editor
    init = function(self)
        -- Get component references here
        -- example:
        -- self.transform = self.entity:require_transform_component()
    end,

    -- Gets called when the game is started
    start = function(self)
    end,

    -- Gets called every frame in the runtime / playtime
    update = function(self, dt)
        -- Check if key is just pressed: 
        -- Input.is_key_just_pressed(Input.Key.Space)

        -- Check if key is held: 
        -- Input.is_key_held(Input.Key.Space)
    end,

    -- Gets called every frame after update in the runtime / playtime
    render = function(self)
        -- Check if key is just pressed: 
        -- Input.is_key_just_pressed(Input.Key.Space)

        -- Check if key is held: 
        -- Input.is_key_held(Input.Key.Space)
    end,
}
