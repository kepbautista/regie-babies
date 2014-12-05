<?php
/**
	Translator Class for converting SQL statements
	to executable statements
**/
class Translator{
	//list of key attributes
	public $key_attributes=array("STUDNO","STUDENT.STUDNO","STUDENTHISTORY.STUDNO",
					  "CNO","COURSE.CNO","COURSEOFFERING.CNO","SEMESTER",
					  "ACADYEAR","SECTION","COURSEOFFERING.SEMESTER",
					  "COURSEOFFERING.ACADYEAR","COURSEOFFERING.SECTION",
					  "STUDCOURSE.STUDNO","STUDCOURSE.CNO","STUDCOURSE.SEMESTER",
					  "STUDCOURSE.ACADYEAR");

	//function for calling the Query Optimizer & Executing a statement
	public function callQueryOptimizer($cmd,$cols,$value){
		//call query optimizer here...
		echo "<br/>-----------------------------------------------------------<br/>";
		echo "command: ".$cmd."<br/>columns: ".$cols;
		echo "<br/>project: ".$_SESSION['project']."<br/>join on: ".$_SESSION['project'];
		echo "<br/>tables: ".$_SESSION['tables']."<br/>values: ".$value;
	}

	//function for translating an INSERT statement
	public function translateInsert($cmd){
		$cols="";$value="";//initialize values for columns and values
		$flag=$ctr=0;//$ctr for flag...
		$columns=$_SESSION['columns'];
		$values=$_SESSION['set_values'];
		$count=count($columns);
		$n_values=count($values);

		//validate column and set_values if they match
		for($ctr=0;$ctr<$count;$ctr++){
			if($ctr==$n_values) break;//empty value can be a null value
			else if(($columns[$ctr]['token_type']!=$values[$ctr]['token_type'])
				&&($values[$ctr]['token_type']!="NULL_TOKEN")){
				$flag = 1;
				break;
			}
		
			//key attribute cannot be a null value
			else if(in_array($columns[$ctr]['lexeme'], $this->key_attributes)
			   &&($values[$ctr]['token_type']=="NULL_TOKEN")){
				$flag = 2;
				break;	
			}
		}

		//incompatible data types
		if($flag==1)
			echo '<br/>Syntax error: Incompatible data type of "'.$columns[$ctr]['lexeme'].'" and "'.$values[$ctr]['lexeme'].'".<br/>';
		//a value for a key attribute is required
		else if($flag==2)
			echo '<br/>Syntax error: Key attribute '.$columns[$ctr]['lexeme'].' cannot be a NULL value.';
		else if($ctr<=$count){
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
			$this->callQueryOptimizer($cmd,$cols,$value);
		}
	}

	//function for translating an UPDATE statement
	public function translateUpdate(){

	}

	//function for translating a DELETE statement
	public function translateDelete(){
		
	}

	//function for translating a SELECT statement
	public function translateSelect(){
		
	}
}
?>