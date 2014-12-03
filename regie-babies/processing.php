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

			/*echo "<br/>----------------------------<br/>";
			print_r($_SESSION);*/

			//construct the translated query here...
			$cmd = $_SESSION['command'];

			$ctr=$count=0;//$ctr for flag...
			if($cmd=="INSERT"){
				$columns=$_SESSION['columns'];
				$values=$_SESSION['set_values'];
				$count=count($values);

				//validate column and set_values if they match
				for($ctr=0;$ctr<$count;$ctr++){
					if($columns[$ctr]['token_type']!=$values[$ctr]['token_type']){
						echo 'Syntax error: Unmatched data type "'.$columns[$ctr]['lexeme'].'" and "'.$values[$ctr]['lexeme'].'".<br/>"';
						break;
					}
				}
			}

			if($ctr==$count){
				/*
					processing columns by concatenating them into one string
					separate each lexeme by a comma
				*/
				
				$cols="";
				$i=0;
				foreach($_SESSION['columns'] as $col){
					if($i==0) $cols.=$col['lexeme'];
					else $cols.=",".$col['lexeme'];
					$i++;
				}

				//call query optimizer here...
			}
			session_unset();//remove all session variables
		}
	}
}

session_destroy();//destroy the session
?>
