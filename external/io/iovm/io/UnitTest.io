//metadoc UnitTest category Test
/*metadoc UnitTest description
An object for organizing and running unit tests validated by assertions.
*/

//doc UnitTest setUp Method called prior to each test.
//doc UnitTest tearDown Method called after each test.
//doc UnitTest testCount Returns the number of tests defined in this object.
//doc UnitTest run Runs all tests.
//doc UnitTest fail Call to trigger a test failure.
UnitTest := Object clone do(
	verbose := method(s,
		//nil
		writeln(s)
	)

	setUp := method(nil)
	
	tearDown := method(nil)
	init := method(self exceptions := List clone)
	testSlotNames := method(
		names := self slotNames select(name, name beginsWithSeq("test")) 
		if(names isEmpty, names, names sortByKey(name, self getSlot(name) message lineNumber))
	)
	
	testCount := method(testSlotNames size)

	run := method(
		testSlotNames foreach(n,
			self setUp
			verbose("    " .. n)
			e := try(stopStatus(self doString(n)))
			if(e,
				write("    ", n, " - failed")
				exceptions append(e)
				write("\n")
				writeln(e showStack)
				//System exit
			)
			//Scheduler currentCoroutine yield
			self tearDown
			//Collector collect
		)
		self ?cleanUp
	)

	fail := method(Exception raise("fail"))

//doc UnitTest assertEquals(a, b) Fail the running test if a != b.
//doc UnitTest assertNotEquals(a, b) Fail the running test if a == b.
//doc UnitTest assertNotSame(a, b) Fail the running test if a != b.
//doc UnitTest assertNil(a) Fail the running test if a != nil.
//doc UnitTest assertNotNil(a) Fail the running test if a == nil.
//doc UnitTest assertTrue(a) Fail the running test if a != true.
//doc UnitTest assertFalse(a) Fail the running test if a != false.
//doc UnitTest assertRaisesException(code) Fail the running test if the code does not raise an Exception.
/*doc UnitTest assertEqualsWithinDelta(expected, actual, delta)
Fail the running test if the expected value is not within delta of the actual value.
*/

	assertEquals := method(a, b, m,
		//writeln("assertEquals1 call message = ", call message type)
		mm := call message
		if(m == nil, m = mm)
		d := m argAt(0) code .. " != " .. call argAt(1) code
		if(a != b, Exception raise("[" .. d .. "] [" .. a asSimpleString .. " != " .. b asSimpleString .. "]"))
		//writeln("assertEquals2")
	)

	assertNotEquals := method(a, b, if(a == b, Exception raise(a asSimpleString .. " == " .. b asSimpleString)))

	assertSame    := method(a, b, assertEquals(a uniqueId, b uniqueId, call message))
	assertNotSame := method(a, b, assertNotEquals(a uniqueId, b uniqueId, call message))
	assertNil     := method(a, assertEquals(a, nil, call message))
	assertNotNil  := method(a, assertNotEquals(a, nil, call message))
	assertTrue    := method(a, assertEquals(a, true, call message))
	assertFalse   := method(a, assertEquals(a, false, call message))

	assertRaisesException := method(
		e := try(
			stopStatus(call evalArgAt(0))
			writeln("Should have raised Exception")
		)
		e ifNil(Exception raise("Should have raised Exception"))
	)

	knownBug := method(
		//writeln("  [known bug: ", call argAt(0) code, "]")
	)

	assertEqualsWithinDelta := method(expected, actual, delta,
		if(((expected - actual) abs > delta),
			Exception raise("expected " .. expected .. " but was " .. actual .. " (allowed delta: " .. delta .. ")")
		)
	)
)

//metadoc TestSuite category Testing
/*metadoc TestSuite description
An object to collect and run multiple UnitTests defined in *Test.io files within the System launchPath directory.
*/
TestSuite := Object clone do(
	verbose := method(s,
		nil
		//writeln(s)
	)

//doc TestSuite name Return the name of the TestSuite.
	name := method(
		path asMutable pathComponent lastPathComponent
	)

	path ::= "."

/*doc TestSuite run
Collects and all files named *Test.io within the System launchPath directory,
runs their tests and prints collated results.
*/
	run := method(
		verbose("\n" .. name)
		unitTestFiles := Directory with(System launchPath) files select(f, f name endsWithSeq("Test.io"))
		exceptions := List clone
		testCount := 0

		unitTestFiles foreach(f,
			1 repeat(
				verbose("  " .. f name fileName)
				test := Lobby doString(f contents, f path)
				//Collector collect
				test run
			)
			testCount = testCount + test testCount
			exceptions appendSeq(test exceptions)
		)

		verbose("  ---------------")
		//Collector collect
		//if(testCount == 0, write("(no tests)"); File standardOutput flush; return(0))
		if(exceptions size > 0) then(
			//writeln("	FAILED ", testCount, " tests, ", exceptions size, " failures\n")
			return(exceptions size)
		) else(
			//writeln("	PASSED ", testCount, " tests\n")
			return(0)
		)
	)
)
