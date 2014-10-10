<?php
class Parser{
	//property declarations

	//add double quotes at the start & end of string
	public function addDoubleQuotes($str){
		return '"'.$str.'"';
	}

	//check if closing and opening symbols match each other
	public function checkMatchingSymbols($query){
		//initialize stack
		$openSymbols=array("(");//list of opening symbols
		$closeSymbols=array(")");//list of closing symbols
		$stack=array();//stack variable
		$msg="";//variable for storing error message

		$quotectr1 = 0;//count number of single quotes
		$quotectr2 = 0;//count number of double quotes

		
		for($i=0;$i<strlen($query);$i++){
			/* Check if opening symbols */
			//if opening symbol, push to stack
			if(in_array($query[$i], $openSymbols)){
				array_push($stack,$query[$i]);
			}
			//if symbol is an opening single quote
			else if(($query[$i]=="'")&&($quotectr1%2==0)){
				array_push($stack,$query[$i]);
				$quotectr1+=1;
			}
			//if symbol is an opening double quote
			else if(($query[$i]=='"')&&($quotectr2%2==0)){
				array_push($stack,$query[$i]);
				$quotectr2+=1;
			}

			/* Check if closing symbols */
			//if closing symbol, check if it matches at the top of stack
			else if((in_array($query[$i], $closeSymbols))
					||(($query[$i]=="'")&&($quotectr1%2!=0))//closing single quote
					||(($query[$i]=='"')&&($quotectr2%2!=0))//closing double quote
					){

				//error in parentheses
				if(empty($stack)||(($query[$i]==')')&&(array_pop($stack)!='('))) {
					$msg="Syntax error: expected '(' or identifier before ')'<br/>";
					break;
				}
				//error in single quotes
				else if(($query[$i]=="'")&&(array_pop($stack)!="'")){
					$msg="Syntax error: expected ' or identifier before ' <br/>";
					break;
				}
				//error in double quotes
				else if(($query[$i]=='"')&&(array_pop($stack)!='"')){
					$msg='Syntax error: expected " or identifier before " <br/>';
					break;
				}
				else if($query[$i]=="'") $quotectr1+=1;//increment number of single quotes
				else if($query[$i]=='"') $quotectr2+=1;//increment number of double quotes
			}
		}

		//stack is not empty and there is an unclosed opening symbol
		if(!empty($stack)) $msg="Syntax error: unclosed ', \" or ( <br/>";

		return $msg;
	}

	public function tokenizeQuery($query){
		$spChars=array("(",")","=",",");//list of special characters
		$tokens=array();
		$tok="";
		$flag1=$flag2=false;//flag1 is for double quotes while flag2 is for single quotes

		for($i=0;$i<strlen($query);$i++){
			if(($flag1==false)&&($query[$i]=='"')){
				$flag1=true;//opening double quote
				$tok=$tok.$query[$i];	
			}
			else if(($flag2==false)&&($query[$i]=="'")){
				$flag2=true;//opening single quote
				$tok=$tok.$query[$i];
			} 
			//processing inside double quotes
			else if($flag1==true){
				if($query[$i]==" ") $tok=$tok." ";
				else if($query[$i]=='"'){
					$flag1=false;//closing quote
					$tok=$tok.$query[$i];
				}
				else $tok=$tok.$query[$i];
			}
			//processing inside single quotes
			else if($flag2==true){
				if($query[$i]==" ") $tok=$tok." ";
				else if($query[$i]=="'"){
					$flag2=false;//closing quote
					$tok=$tok.$query[$i];
				}
				else $tok=$tok.$query[$i];
			}
			else{
				//ignore whitespaces if not found inside quotes
				if($query[$i]==" "){
					//push token if not empty string
					if($tok!="") array_push($tokens,$tok);
					$tok="";
				}
				//read "=",",","(" or ")" sign
				else if(in_array($query[$i], $spChars)){
					if($tok!="") array_push($tokens,$tok,$query[$i]);
					else array_push($tokens, $query[$i]);
					$tok="";
				}
				//end of query ";"
				else if($query[$i]==";"){
					if($tok!="") array_push($tokens,$tok,";");
					else array_push($tokens, ";");
					//start new query
				}
				else $tok=$tok.$query[$i];
			}
		}

		return $tokens;
	}
}
?>