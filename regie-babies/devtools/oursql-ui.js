$(document).ready(function(){
		var phpScript='processing.php';

		$('#execute_query').click(function(){
                $.post(phpScript, {
            query: $('#query_input').val()
	        },
	        function(data){
	            $("#query_result").html(data);
	        });
        });
});