--
-- Tiny library to parse out command line arguments
--

-- Library for parsing out the command line arguments
local cli = {}


do

    -- Helper function to be called initially to parse out the cli args
    --- @return table parsed_args The parsed arguments
    --- @return table remaining The unparsed remaining inptus
    local function simple_parse_args(...)

        local _rawArgs = { ... }
        local _parsedArgs = {}

        -- Loop until we hit the first option or the end
        for i, v in ipairs(_rawArgs) do
            -- Check for option prefix
            if string.sub(v, 1, 1) ~= "-" then
                -- Not an option, add to args
                _parsedArgs[#_parsedArgs+1] = v
            else
                -- Found an option, stop parsing
                goto DONE_PARSING_ARGS
            end
        end
        ::DONE_PARSING_ARGS::

        -- Remaining args are everything left after parsing
        local _remaining = { table.unpack(_rawArgs, #_parsedArgs + 1) }

        -- Return results
        return _parsedArgs, _remaining
    end

    -- Helper function to be called after parsing the args from the cli args table
    --- @return table options The parsed options
    local function simple_parse_ops(...)

        local _rawOps = { ... }

        -- The parsed options table
        local _ops = {}

        -- The name of the last parsed option, used to set
        -- options' values
        local _lastParsedName

        -- Loop through raw ops input
        for i, v in ipairs(_rawOps) do

            -- Check if this is an option name
            if string.sub(v, 1, 1) == "-"  then

                -- Check if this is a long option
                if string.sub(v, 1, 2) == "--" then

                    -- Long option found
                    local _name = string.sub(v, 3)

                    -- Add to options table
                    _ops[_name] = true

                    -- Set last parsed option name in case a value is next
                    _lastParsedName = _name

                else

                    -- Short option found
                    local _names = string.sub(v, 2)
                    local _namesLen = string.len(_names)

                    -- Check for single character
                    if _namesLen == 1 then

                        -- Single letter option defined
                        local _name = _names

                        -- Add to options table
                        _ops[_name] = true

                        -- Set last parsed option name in case a value is next
                        _lastParsedName = _name

                    else

                        -- Multiple option names defined
                        -- Loop through each character and add to table
                        for n = 1, _namesLen do
                            -- Add to options table
                            _ops[string.sub(_names, n, n)] = true
                        end

                        -- Clear last parsed option name
                        _lastParsedName = nil

                    end

                end

            else
                -- Check that we parsed a name last
                assert(_lastParsedName ~= nil, "option argument \"" .. v .. "\" not proceeded by option name!")

                -- Set last option value to this string
                _ops[_lastParsedName] = v

                -- Clear last parsed name
                _lastParsedName = nil

            end

        end

        -- All done parsing
        return _ops
    end

    -- Check if a string is an end quote
    local function is_end_quote(_str)
        local _len = string.len(_str)
        if _len > 1 then
            -- Make sure it isn't an escaped quote
            local _last2 = string.sub(_str, _len - 1, _len)
            return string.sub(_last2, 2, 2) == "\"" and string.sub(_last2, 1, 1) ~= "\\"
        else
            -- Check if quote
            return string.sub(_str, 1, 1) == "\""
        end
    end

    -- Concats multi-word arguments in quotes into single args
    --- @return table raw_args The corrected command line arguments
    local function concat_quoted_cli_args(...)
        local _rawArgs = { ... }
        local _args = {}
        local _currentQuoted = nil

        -- Loop through and concat quoted args
        for i, v in ipairs(_rawArgs) do
            local _len = string.len(v)
            if string.sub(v, 1, 1) == "\"" then
                if _currentQuoted ~= nil and _len == 1 then
                    -- End of quoted with single quote
                    _currentQuoted = _currentQuoted .. " " .. v
                    _args[#_args+1] = _currentQuoted
                    _currentQuoted = nil
                else
                    -- Start of quoted
                    assert(_currentQuoted == nil)
                    _currentQuoted = v
                end
            elseif is_end_quote(v) then
                -- End of quoted
                assert(_currentQuoted)
                _currentQuoted = _currentQuoted .. " " .. v
                _args[#_args+1] = _currentQuoted
                _currentQuoted = nil
            elseif _currentQuoted ~= nil then
                -- Add to quoted
                _currentQuoted = _currentQuoted .. " " .. v
            else
                -- Add to args
                _args[#_args+1] = v
            end
        end

        return _args
    end

    ---
    --- Parses command line arguments into arguements and options.
    ---
    --- CLI args are considered options if they begin with a '-' character.
    ---
    --- The following is a list of examples of how parsing behaves. The left hand
    --- side contains the inputs to the parse function as table of strings. The right
    --- hand side shows the arguments ("args") and options ("ops") tables returned from the parse function.
    --- Each example is numbered and shows what the input cli args looks like from the command line.
    ---
    --- Arguements are always assumed to come first, with options being listed afterwards.
    ---
    ---     1. hello! -a
    ---      { "hello!", "-a" } -> args = { "hello!" }, ops = { a = true }
    ---
    ---     2. hello! you are bad -a
    ---      { "hello!", "you", "are", "bad", "-a" } -> args = { "hello!", "you", "are", "bad" }, ops = { a = true }
    ---
    ---     3. hello! --a
    ---      { "hello!", "--a" } -> args = { "hello!" }, ops = { a = true }
    ---
    ---     4. hello! -afg
    ---      { "hello!", "-afb" } -> args = { "hello!" }, ops = { a = true, f = true, b = true }
    ---
    ---     5. hello! --name your_mom
    ---      { "hello!", "--name", "your_mom" } -> args = { "hello!" }, ops = { name = "your_mom" }
    ---
    ---     6. why are you running -a --name your_mom
    ---      { "why", "are", "you", "running", "-a", "--name", "your_mom" } -> args = { "why", "are", "you", "running" }, ops = { a = "true", name = "your_mom" }
    ---
    ---
    --- @param ... any The command line arguments as provided at the beginning of the script.
    ---
    --- @return table args  Contains an integer indexed table with arguments.
    --- @return table ops   Contains a string index table with option's values. The value is true if it was provided, and a string if an argument was provided.
    ---
    function cli.parse(...)

        -- Concat quoted args into single
        local _rawArgs = concat_quoted_cli_args(...)

        -- Parse the arguments
        local _args, _rawOptions = simple_parse_args(table.unpack(_rawArgs))

        -- If there are options to parse, parse them
        local _ops = {}
        if #_rawOptions ~= 0 then
            _ops = simple_parse_ops(table.unpack(_rawOptions))
        end

        -- All done, return tables
        return _args, _ops
    end

end

return cli