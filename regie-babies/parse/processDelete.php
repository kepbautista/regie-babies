<?php
/**
	PHP File for processing DELETE statements
**/
include('processWhereStmt.php');
class ProcessDelete extends ParseProcess{
	//parse start of DELETE Statement
	public function parseDelete($stmt,$index){
		//echo $stmt[$index]['token']." ";
		
		if($index<count($stmt)){
			//current lexeme and token
			$token=$stmt[$index]['token'];
			$lexeme=$stmt[$index]['lexeme'];

			//next lexeme and token
			$nextTok=$this->getNextToken($stmt,$index);
			$nextLex=$this->getNextLexeme($stmt,$index);

			//evaluate each token
			switch($token){
				case "DELETE_COMMAND": //FROM clause
						if($nextTok=="TABLE_SELECT_OPERATOR")
							$this->parseDelete($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "TABLE_SELECT_OPERATOR": //FROM clause
						if($nextTok=="TABLE_NAME")
							$this->parseDelete($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "TABLE_NAME": //name of table
						$_SESSION['tables']=strtoupper($lexeme);
						if($nextTok=="SELECT_OPERATOR"){
							$p = new ProcessWhereStmt();
							$p->parseWhereStmt($stmt,$index+1);
						}
						else if($nextTok=="END_OF_STATEMENT") break;//end of the statement
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
						
			}
		}
	}
}
?>