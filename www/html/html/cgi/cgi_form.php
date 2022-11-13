<?php
	session_start();
?>

<!DOCTYPE html>
<html>
	<head>
		<title>Cutie webserv 😚 💖</title>
		<meta charset="UTF-8">
		<link href="cgi_form.css" rel="stylesheet">
		<link rel="icon" href="../img/favicon.ico">
	</head>
	<body class="background">
		<div class="icon">
			<a href="../form_order"><img src="../img/icon_panier_no_cgi.png" width="100px"></a>
		</div>
		<div class="icon_cgi">
			<a href="cgi_achat.php"><img src="../img/icon_panier_with_cgi.png" width="100px"></a>
		</div>
		<div class="paint">
			<a href="../form_gallery"><img src="../img/icon_paint.png" width="100px"></a>
		</div>
		<div class="house">
			<a href="../index.html"><img src="../img/icon_house.png" width="100px"></a>
		</div>
		<div class="danse">
			<a href="http://gcasale.free.fr/viva.htm"><img src="../img/icon_dance.gif" width="90px"></a>
		</div>
		<h1>
			TON HAMSTER EST PRET !!!
		</h1>
		<div class="hamster">
			<img src="../img/hamster_party.png" width="600px">
		</div>
		<div class="inscription">
		<?php
			if (isset($_SERVER['REQUEST_METHOD']))
			{
				switch($_SERVER['REQUEST_METHOD'])
				{
					case 'GET':
						$_SESSION['cart'][] = array(
							'name' => $_GET['name'],
							'hamster' => $_GET['hamster'],
							'color' => $_GET['color']
						);
						echo 'Votre nom : '.$_GET["name"].'<br>';
						echo 'Le nom de votre futur hamster : '.$_GET["hamster"].'<br>';
						echo 'Vous avez choisi sa couleur magnifique qui est le '.$_GET["color"].'<br>';
						break;
					case 'POST':
						$_SESSION['cart'][] = array(
							'name' => $_POST['name'],
							'hamster' => $_POST['hamster'],
							'color' => $_POST['color']
						);
						echo 'Votre nom : '.$_POST["name"].'<br>';
						echo 'Le nom de votre futur hamster : '.$_POST["hamster"].'<br>';
						echo 'Vous avez choisi sa couleur magnifique qui est le '.$_POST["color"].'<br>';
						break;
					default:
						echo "Il faut un minimum d'infos pour obtenir votre petit bebou!\n";
				}
			}
		?>
		</div>
		<form action="../form.html">
			<center><input type="submit" value="Go acheter un nouvel ami !" class="bouton" /></center>
		</form>
		<form action="cgi_achat.php">
			<center><input type="submit" value="Go voir le panier achete !" class="bouton" /></center>
		</form>
        <form action="cgi_destroy.php">
			<center><input type="submit" value="Jeter votre hamster et reformuler une adoption" class="bouton" /></center>
		</form>
		<form action="../index.html">
			<center><input type="submit" value="Retour vers la page d'accueil" class="bouton" /></center>
		</form>
	</body>
</html>




