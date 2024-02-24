local uunit = {
  _VERSION = "uunit.lua v1.0.0",
  _DESCRIPTION = "micro unit testing library in Lua",
  _URL = "https://github.com/superzazu/uunit.lua",
  _LICENSE = [[zlib License

(C) 2024 Nicolas ALLEMAND

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.]],
}

---@class UunitTest
---@field name string The test name
---@field func function Test function
---@field skipped boolean Whether test must be skipped
---@field todo boolean Whether test must be marked as "todo"

---@class UunitTestSuite
---@field name string The test suite name
---@field private _tests UunitTest[] Array of tests to be run
---@field beforeAll function? Function to be executed before tests
---@field afterAll function? Function to be executed after all tests
---@field beforeEach function? Function to be executed before each test
---@field afterEach function? Function to be executed after each test
---@field test fun(suite: UunitTestSuite, testName: string, testFunc: function) Adds a test to the test suite
---@field skip fun(suite: UunitTestSuite, testName: string, testFunc: function) Adds a "skipped" test to the test suite
---@field todo fun(suite: UunitTestSuite, testName: string, testFunc: function) Adds a "todo" test to the test suite

---@class UunitResults
---@field failedTests any[] Array of failed tests
---@field nbTestsExecuted number Number of tests executed
---@field nbTestsFailed number Number of failed tests

---@class UunitOptions
---@field failFast boolean Whether to stop running tests when a test failed
---@field shuffleTests boolean Whether to shuffle tests (randomize execution order)

---@class UunitFormatter
---@field before fun()
---@field after fun(results: UunitResults)
---@field each fun(test: UunitTest, ok: boolean, error: string|nil)
---@field skip fun(test: UunitTest)
---@field todo fun(test: UunitTest)

---Shuffles a table
---@param t any[] Table to shuffle
local shuffle = function(t)
  for i = #t, 2, -1 do
    local j = math.random(i)
    t[i], t[j] = t[j], t[i]
  end
end

---@type UunitOptions
uunit.defaultOptions = {failFast = false, shuffleTests = true}

---Returns the default formatter.
---@param suite UunitTestSuite
---@param options UunitOptions
---@return UunitFormatter
uunit.defaultFormatter = function(suite, options)
  ---@type UunitFormatter
  local formatter = {
    before = function()
      io.write(string.format("> %s:\n", suite.name))
    end,
    after = function(results)
      local CRED = "\x1B[31m"
      local CGRN = "\x1B[32m"
      local CBLU = "\x1b[34m"
      local CRST = "\x1B[0m"

      io.write("\n")

      if results.nbTestsFailed > 0 then
        io.write("\nFAILURES:\n")
        for i = 1, #results.failedTests do
          io.write(string.format("- %s: %s\n", results.failedTests[i].test.name,
              results.failedTests[i].error))
        end
        io.write("\n")
      end

      io.write(string.format(
          "%s%d%s successes / %s%d%s failures / %s%d%s total%s", CGRN,
          results.nbTestsExecuted - results.nbTestsFailed, CRST, CRED,
          results.nbTestsFailed, CRST, CBLU, results.nbTestsExecuted, CRST,
          options.failFast and " (failfast)" or ""))
    end,
    each = function(test, ok, error)
      io.write(ok and "." or "F")
    end,
    skip = function(test)
      io.write("S")
    end,
    todo = function(test)
      io.write("T")
    end,
  }
  return formatter
end

---Creates a new test suite
---@param suiteName string The test suite name
---@return UunitTestSuite
uunit.newSuite = function(suiteName)
  ---@type UunitTestSuite
  local suite = {
    name = suiteName,
    _tests = {},
    beforeAll = nil,
    beforeEach = nil,
    afterAll = nil,
    afterEach = nil,
    test = function(s, testName, testFunc)
      s._tests[#s._tests + 1] = {
        name = testName,
        func = testFunc,
        skipped = false,
        todo = false,
      }
    end,
    skip = function(s, testName, testFunc)
      s._tests[#s._tests + 1] = {
        name = testName,
        func = testFunc,
        skipped = true,
        todo = false,
      }
    end,
    todo = function(s, testName, testFunc)
      s._tests[#s._tests + 1] = {
        name = testName,
        func = testFunc,
        skipped = false,
        todo = true,
      }
    end,
  }

  -- function suite:todo(testName, testFunc)
  --   self._tests[#self._tests + 1] = {
  --     name = testName,
  --     func = testFunc,
  --     skipped = false,
  --     todo = true,
  --   }
  -- end

  -- ---@type UunitTestSuite
  -- local suite = {}
  -- suite.name = suiteName
  -- suite._tests = {}
  -- suite.beforeAll = nil
  -- suite.beforeEach = nil
  -- suite.afterAll = nil
  -- suite.afterEach = nil

  -- suite.test = function(s, testName, testFunc)
  -- end

  -- suite.skip = function(suite, testName, testFunc)
  -- end

  -- suite.todo = function(suite, testName, testFunc)
  -- end

  return suite
end

---Runs a test suite
---@param suite UunitTestSuite Test suite to run
---@param formatterGetter? fun(suite: UunitTestSuite, options: UunitOptions): UunitFormatter
---@param options? UunitOptions
---@return UunitResults
uunit.run = function(suite, formatterGetter, options)
  formatterGetter = formatterGetter or uunit.defaultFormatter
  options = options or uunit.defaultOptions

  local formatter = formatterGetter(suite, options)

  ---@type UunitResults
  local results = {failedTests = {}, nbTestsExecuted = 0, nbTestsFailed = 0}

  local tests = suite._tests

  if options.shuffleTests then
    math.randomseed(os.time())

    tests = {}
    for i = 1, #suite._tests do
      tests[i] = suite._tests[i]
    end

    shuffle(tests)
  end

  formatter.before()
  if suite.beforeAll then
    suite.beforeAll()
  end

  for i = 1, #tests do
    results.nbTestsExecuted = results.nbTestsExecuted + 1

    if tests[i].skipped then
      formatter.skip(tests[i])
    elseif tests[i].todo then
      formatter.todo(tests[i])
    else
      if suite.beforeEach then
        suite.beforeEach()
      end
      local ok, error = pcall(tests[i].func)
      formatter.each(tests[i], ok, error)
      if suite.afterEach then
        suite.afterEach()
      end

      if not ok then
        local failedTest = {test = tests[i], error = error}
        results.failedTests[#results.failedTests + 1] = failedTest

        if options.failFast then
          break
        end
      end
    end
  end

  results.nbTestsFailed = #results.failedTests

  if suite.afterAll then
    suite.afterAll()
  end
  formatter.after(results)

  return results
end

-- asserters

---Asserts a value is true
---@param a any
uunit.assertTrue = function(a)
  if not (a == true) then
    error(string.format("`%s` is not true", a), 2)
  end
end

---Asserts a value is false
---@param a any
uunit.assertFalse = function(a)
  if not (a == false) then
    error(string.format("`%s` is not false", a), 2)
  end
end

---Asserts a value is truthy
---@param a any
uunit.assertTruthy = function(a)
  if not (a ~= nil and a ~= false) then
    error(string.format("`%s` is not truthy", a), 2)
  end
end

---Asserts a value is falsy
---@param a any
uunit.assertFalsy = function(a)
  if not (a == nil or a == false) then
    error(string.format("`%s` is not falsy", a), 2)
  end
end

---Asserts a & b are equal
---@param a any
---@param b any
uunit.assertEq = function(a, b)
  if not (a == b) then
    error(string.format("`%s` does not equal `%s`", a, b), 2)
  end
end

---Asserts a & b are not equal
---@param a any
---@param b any
uunit.assertNotEq = function(a, b)
  if not (a ~= b) then
    error(string.format('expected `%s` to be different than `%s`', a, b), 2)
  end
end

---Asserts a is greater than b
---@param a any
---@param b any
uunit.assertGt = function(a, b)
  if not (a > b) then
    error(string.format("expected `%s` to be greater than `%s`", a, b), 2)
  end
end

---Asserts a is lesser than b
---@param a any
---@param b any
uunit.assertLt = function(a, b)
  if not (a < b) then
    error(string.format("expected `%s` to be lesser than `%s`", a, b), 2)
  end
end

---Asserts a is greater or equal b
---@param a any
---@param b any
uunit.assertGte = function(a, b)
  if not (a >= b) then
    error(
        string.format("expected `%s` to be greater (or equal) than `%s`", a, b),
        2)
  end
end

---Asserts a is lesser or equal b
---@param a any
---@param b any
uunit.assertLte = function(a, b)
  if not (a <= b) then
    error(
        string.format("expected `%s` to be lesser (or equal) than `%s`", a, b),
        2)
  end
end

---Asserts a is in b (strings)
---@param a string
---@param b string
uunit.assertStrIn = function(a, b)
  if b:find(a, 1, false) == nil then
    error(string.format('`%s` not found in `%s`', a, b), 2)
  end
end

---Asserts a string matches a pattern
---@param str string
---@param pattern string
uunit.assertStrMatch = function(str, pattern)
  if str:find(pattern) == nil then
    error(string.format('`%s` does not match `%s`', str, pattern), 2)
  end
end

---Asserts a & b are almost equal
---@param a number
---@param b number
---@param diff? number
uunit.assertAlmostEq = function(a, b, diff)
  diff = diff or 0.00000000001
  if not (math.abs(a - b) <= diff) then
    error(string.format('`%s` is not almost equal to `%s`', a, b), 2)
  end
end

---Asserts function f raises a given error
---@param f function The function that should raise an error
---@param expectedError string The error that should be raised
---@param disablePattern? boolean Truns off the pattern matching facilities when comparing "expectedError" with error
uunit.assertRaises = function(f, expectedError, disablePattern)
  disablePattern = disablePattern or true
  local ok, err = pcall(f)
  if ok then
    error(string.format('error `%s` not raised', expectedError), 2)
  elseif expectedError and err:find(expectedError, 1, disablePattern) == nil then
    error(string.format('`%s` not found in error `%s`', expectedError, err), 2)
  end
end

---Asserts a value is nil
---@param a any
uunit.assertNil = function(a)
  if not (a == nil) then
    error(string.format('`%s` is not nil', a), 2)
  end
end

---Asserts a value is not nil
---@param a any
uunit.assertNotNil = function(a)
  if not (a ~= nil) then
    error(string.format('expected `%s` to be nil', a), 2)
  end
end

---Asserts a value is of a given type
---@param a any
---@param t type
uunit.assertType = function(a, t)
  if not (type(a) == t) then
    error(string.format('expected `%s` to be of type `%s` (is %s)', a, t,
        type(a)), 2)
  end
end

local isSame = nil
isSame = function(a, b)
  if type(a) ~= type(b) then
    error(string.format("expected `%s` and `%s` to have the same type", a, b))
  end

  if type(a) == 'table' then
    for k, _ in pairs(a) do
      ---@diagnostic disable-next-line: need-check-nil
      isSame(a[k], b[k])
    end
    for k, _ in pairs(b) do
      ---@diagnostic disable-next-line: need-check-nil
      isSame(a[k], b[k])
    end
  else
    if a ~= b then
      error(string.format("`%s` does not equal `%s`", a, b, 2))
    end
  end
end

---Asserts two values are the same. If both a & b are tables, will recursively
---check each property on the objects.
---@param a any
---@param b any
uunit.assertSame = function(a, b)
  return isSame(a, b)
end

return uunit
