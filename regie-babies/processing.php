<?php
//classes
include("parser.php");

//list of key attributes
$key_attributes=array("STUDNO","STUDENT.STUDNO","STUDENTHISTORY.STUDNO",
					  "CNO","COURSE.CNO","COURSEOFFERING.CNO","SEMESTER",
					  "ACADYEAR","SECTION","COURSEOFFERING.SEMESTER",
					  "COURSEOFFERING.ACADYEAR","COURSEOFFERING.SECTION",
					  "STUDCOURSE.STUDNO","STUDCOURSE.CNO","STUDCOURSE.SEMESTER",
					  "STUDCOURSE.ACADYEAR");

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
			$cols="";$value="";
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
				$count=count($columns);
				$n_values=count($values);

				//validate column and set_values if they match
				for($ctr=0;$ctr<$count;$ctr++){
					if($ctr==$n_values) break;//empty value can be a null value
					else if(($columns[$ctr]['token_type']!=$values[$ctr]['token_type'])
						&&($values[$ctr]['token_type']!="NULL_TOKEN")){
						echo 'Syntax error: Incompatible data type of "'.$columns[$ctr]['lexeme'].'" and "'.$values[$ctr]['lexeme'].'".<br/>';
						break;
					}
					//key attribute cannot be a null value
					else if(in_array($columns[$ctr]['lexeme'], $key_attributes)
						   &&($values[$ctr]['token_type']=="NULL_TOKEN")){
						echo 'Syntax error: Key attribute '.$columns[$ctr]['lexeme'].' cannot be a NULL value.<br/>';
						break;	
					}

				}

				if($ctr<=$count){
					/*
						processing columns by concatenating them into one string
						separate each lexeme by a comma
					*/				
					
					$i=0;
					foreach($_SESSION['columns'] as $col){
						if($i==0) $cols.=$col['lexeme'];
						else $cols.=",".$col['lexeme'];
						$i++;
					}

					/*
						processing values by concatenating them into one string
						separate each lexeme by a comma
					*/
					$no_of_values=count($_SESSION['columns']);//number of values to be processed
					for($i=0;$i<$no_of_values;$i++){
						if($i==0) $value.=$values[$i]['lexeme'];
						else if($i>=count($_SESSION['set_values'])) $value.=",NULL";
						else $value.=",".$values[$i]['lexeme'];
					}
					$parse->callQueryOptimizer($cmd,$cols,$value);
				}				
			}
			session_unset();//remove all session variables
		}
	}
}

session_destroy();//destroy the session
?>
