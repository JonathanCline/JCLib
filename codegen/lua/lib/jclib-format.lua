local format = {}

function format.format(_fmt, ...)
    local _args = { ... }
    local _idx = 1

    local _out = string.gsub(_fmt, "[^\\]{[^}]*}", function(_str)
        local _firstChar = string.sub(_str, 1, 1)
        _str = string.sub(_str, 2)
        if _str == "{}" then
            -- Auto indexed
            assert(_idx ~= nil, "cannot mix auto-indexed and manually indexed formatting arguments")
            assert(_args[_idx] ~= nil, "not enough formatting arguments")
            _idx = _idx + 1
            return _firstChar .. tostring(_args[_idx - 1])
        else
            local _strContents = string.sub(_str, 2, -2)
            if string.match(_strContents, "%d+") then
                -- Manually indexed
                local _manualIdx = tonumber(_strContents)         
                assert(_args[_manualIdx] ~= nil, "not enough formatting arguments")
                _idx = nil
                return _firstChar .. tostring(_args[_manualIdx])
            else
                assert(false, "unrecognized formatting argument")
                return _firstChar
            end
        end

    end)
    
    return _out
end


setmetatable(format,
{
    __call = function(self, _fmt, ...)
        return self.format(_fmt, ...)
    end
})
return format