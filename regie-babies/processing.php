<?php
//classes
include("parser.php");

//start the session
session_start();

//instantiate classes
$parse = new Parser();//parser class

$query=trim(addslashes($_POST['query']));

//check if matching symbols (quotes)
$msg=$parse->checkMatchingQuotes($query);

//there is an error
if($msg!="")
	echo $msg;
else{

	//tokenize input and group according to statements
	$toks=$parse->tokenizeQuery($query);
	
	//perform lexical analysis
	$stmts = $parse->lexer($toks);
	$parse->printLex($stmts);//print lexemes and tokens table

	//check if matching symbols (parenthesis)
	$msg=$parse->checkMatchingSymbols($stmts);

	//there is an error in opening symbols
	if($msg!=""){
		echo $msg;
	}
	else{
		foreach ($stmts as $stmt) {
			/* parts of the statement to be passed to the query optimizer (query_opt.c) */
			$_SESSION['command']=""; //command to be executed (ok)
			$_SESSION['columns']=array(); //columns involved
			$_SESSION['project']=""; //select (where) condition
			$_SESSION['join_on']=""; //join conditions
			$_SESSION['tables']=""; //table names
			$_SESSION['set_values']=array(); //values for update or insert

			$parse->parseExpression($stmt);

			echo "<br/>----------------------------<br/>";
			print_r($_SESSION);
			session_unset();//remove all session variables
		}

		/**
			Part for passing arguments to executable files
		**/
		/*add double quotes at the start & end of string
	  	for passing to command line arguments in the executable
	  	files
		*/
		//$query=$parse->addDoubleQuotes($query);

		/*exec("./oursql {$query}",$out1);//run C++ program
		exec("java Echo {$query}",$out2);//run Java program

		print_r($out1);
		print_r($out2);*/

		/**
			call query optimizer here...
	      and call storage manager inside query optimizer
		**/
	  //exec("exec_files/query_opt {'command...'}",$out1);
		//$out1 = shell_exec("/test");
	}
}

session_destroy();//destroy the session
?>
