<?php
$verb = $_SERVER['REQUEST_METHOD'];
$uri = $_SERVER['REQUEST_URI'];

switch($uri){
	case "/411Api/image/getObject":handleObjectFind($verb); break;
	default: break;
}





function handleObjectFind($verb){
	switch($verb){
		case "POST":findGenericObject();break;
	}
}



function findGenericObject(){
	$dat = json_decode(file_get_contents("php://input"), true);
	$im = imagecreatefromstring(base64_decode($dat['image']));
	$res = imagejpeg($im, "sentImage.jpg");
	$result = exec('python findObjectServer.py square red sentImage.jpg');
	unlink("sentImage.jpg");
	echo json_encode($result);
}


?>

