<?php
/**
	PHP File for Join condition processing
**/
class ProcessJoin extends ParseProcess{
	// parse start of join condition
	public function parseJoinStmt($stmt,$index){
		//echo $stmt[$index]['token']." ";
		$table = $_SESSION['tables'];//get names of tables

		if($index<count($stmt)){
			//current lexeme and token
			$token=$stmt[$index]['token'];
			$lexeme=$stmt[$index]['lexeme'];

			//next lexeme and token
			$nextTok=$this->getNextToken($stmt,$index);
			$nextLex=$this->getNextLexeme($stmt,$index);

			//evaluate each token
			switch($token){
				case "JOIN_CONDITION_STATEMENT":
						break;
				
			}
		}
	}
}
?>