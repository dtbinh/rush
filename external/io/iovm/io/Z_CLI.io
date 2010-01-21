CLI := Object clone do(
	prompt ::= "Io> "
	outPrompt ::= "==> "
	continuedLinePrompt ::= "    "

	context ::= method(Lobby)
	isRunning ::= true

	commandLineLabel ::= "Command Line"

	lineReader ::= nil

	stop := method(setIsRunning(false))

	runFile := method(path,
		System launchPath := if(Path isPathAbsolute(path),
			path
		,
			Directory currentWorkingDirectory asMutable appendPathSeq(path)
		) pathComponent

		System launchScript = path

		e := try(context doFile(path)) 
		if(e, e showStack; return -1)
		return 0
	)

	runIorc := method(
		home := System getEnvironmentVariable("HOME")
		if(home,
			path := Path with(home, ".iorc")
			if(File with(path) exists,
				context doFile(path)
			)
		)
	)

	ioHistoryFile := method(
		Path with(System getEnvironmentVariable("HOME"), ".io_history")
	)

	saveHistory := method(
		lineReader ?saveHistory(ioHistoryFile)
	)

	loadHistory := method(
		if(File with(ioHistoryFile) exists,
			lineReader ?loadHistory(ioHistoryFile)
		)
	)

	run := method(		
		System launchPath := Directory currentWorkingDirectory
		Importer addSearchPath(System launchPath)
		context exit := method(
			CLI saveHistory
			System exit
		)
		
		runIorc

		if(System ?args first == "-e",
			writeln(context doString(System args exSlice(1) map(asUTF8) join(" ")))
			return
		)

		if(System ?args and System args size > 0,

			if(System args first == "-i",
				if(System args size >= 2,
					return runFile(System args at(1))
				,
					if(File clone setPath("main.io") exists, runFile("main.io"))
				)
				return interactiveMultiline
			)

			runFile(System args first)
		,
			if(File clone setPath("main.io") exists,
				runFile("main.io")
			,
				interactiveMultiline
			)
		)
	)

	writeWelcomeBanner := method(
		writeln("Io ", System version)
	)

	interactiveMultiline := method(
		writeWelcomeBanner

		// Use GNU Readline as the default line reader. Fall back to Editline 
		//try(setLineReader(ReadLine))
		//try(lineReader ifNil( setLineReader(EditLine)))

		//loadHistory // don't inable this unless you intend to make it work properly on osx

		while(isRunning,
			if(lineReader isNil not,
				handleInteractiveMultiline
			,
				interactiveNoLineReader
			)
		)

		//saveHistory
	)

	interactive := method(
		writeWelcomeBanner
		while(isRunning,
			handleInteractiveSingleLine
		)
	)

	writeCommandResult := method(result,
		e := try(string := getSlot("result") asString)
		if(e not,
			writeln(outPrompt, string)
		,
			writeln(outPrompt, "<exception while dislaying result>")
			e showStack
		)
	)

	/*
	handleInteractiveSingleLine := method(
		line := EditLine readLine(prompt)
		line ifNil(
			writeln
			context exit
		)

		EditLine addHistory(line)

		e := try(result := context doMessage(line asMessage(commandLineLabel)))
		if(e,
			e showStack
		,
			writeCommandResult(getSlot("result"))
		)
	)
	*/

	interactiveNoLineReader := method(
		write(prompt)
		File standardOutput flush

		line := File standardInput readLine

		line ifNil(
			writeln
			context exit
		)

		e := try(result := context doMessage(line asMessage(commandLineLabel)))
		if(e,
			e showStack
		,
			writeCommandResult(getSlot("result"))
		)
	)

	errorMessage := method(error,
		error beforeSeq(" on line")
	)

	# Find error messages for the errors we understand
	lazySlot("knownErrors",
		m := Map clone
		m atPut(compileErrorMessage("("), ")-> ")
		m atPut(compileErrorMessage("["), "]-> ")
		m atPut(compileErrorMessage("{"), "}-> ")
		m atPut(compileErrorMessage("\"\"\""), "\"-> ")
		m atPut(knownErrorMissingArgument, ")-> ")
	)

	lazySlot("knownErrorMissingArgument",
		compileErrorMessage("(x,")
	)

	compileErrorMessage := method(source,
		errorMessage(try(source asMessage) error)
	)

	handleInteractiveMultiline := method(
		# Start with the default prompt. The prompt is changed for continued lines, and errors.
		nextPrompt := prompt
		line := ""

		# If there are unmatched ( or the command ends with a \ then we'll need to read multiple lines
		loop(
			# Write out prompt and read line
			nextLine := lineReader readLine(nextPrompt)

			# If there was no line, exit
			nextLine ifNil(context exit)

			# Add what we read to the line we've been building up
			if(nextLine size > 0,
				if(line size > 0,
					line = line .. nextLine
				,
					line = nextLine
				)
			)

			# If there is a \ on the end of the line, then keep building up the line
			if(line endsWithSeq("""\\"""),
				nextPrompt = continuedLinePrompt
				continue
			)

			compileError := try(lineAsMessage := line asMessage(commandLineLabel))
			if(compileError,
				if(nextLine size > 0,
					# If they're missing the end of the line, then let them finish it
					error := compileError error
					continuePrompt := knownErrors at(errorMessage(error))
					if(error == knownErrorMissingArgument and line asMutable strip endsWithSeq(",") not,
						continuePrompt = nil
					)
					if(continuePrompt,
						nextPrompt = continuePrompt
						continue
					)
				)

				# If the error can't be fixed by continuing the line, report the error.
				compileError showStack
				lineReader addHistory(line)
				return
			)

			lineReader addHistory(line)

			# Execute the line and report any exceptions which happen
			executionError := try(result := context doMessage(lineAsMessage, context))
			if(executionError,
				executionError showStack
				return
			,
				# Write out the command's result
				writeCommandResult(getSlot("result"))
				return
			)
		)
	)
)
