<?php
//classes
include("parser.php");
include("translator.php");

//start the session
session_start();

//instantiate classes
$parse = new Parser();//create an instance of the parser class

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
			$_SESSION['select']=""; //select (where) condition
			$_SESSION['join_on']=""; //join conditions
			$_SESSION['tables']=""; //table names
			$_SESSION['set_values']=array(); //values for update or insert
			$_SESSION['error']=0;//if there is a syntax error
			$_SESSION['temp_type']="";//temporary variable for storing token_type
			$_SESSION['temp_lex']="";//temporary variable for storing lexemes

			$table_ctr = 0;

			//count number of tables inside the statement
			foreach($stmt as $value){
				if($value['token']=="TABLE_NAME")
					$table_ctr += 1;
			}

			$parse->parseExpression($stmt);//parse the expression if it follows correct syntax

			//construct the translated query here...
			$cmd = $_SESSION['command'];

			$t = new Translator();//create an instance of the Translator class
			//print_r($_SESSION);
			
			//no errors were found
			if($_SESSION['error']==0){
				// translate INSERT or UPDATE statement
				if($cmd=="INSERT"||$cmd=="UPDATE") $t->translateSetValues($cmd);
				else if($cmd=="DELETE") $t->translateDelete($cmd); // translate DELETE statement
				else if($cmd=="SELECT"){
					$t->translateSelect($cmd,$table_ctr); 
				}//translate SELECT statement				
			}
			else break;//there are errors found in the code so it should not run
		}
	}
}

session_destroy();//destroy the session
?>
