<!DOCTYPE html>
<?php
	session_start();
?>
<html>
	<head>
		<title>Cutie webserv 😚 💖</title>
		<meta charset="UTF-8">
        <link href="cgi_achat.css" rel="stylesheet">
		<link rel="icon" href="../img/favicon.ico">
	</head>
	<body class="background">
		<div class="icon">
			<a href="../form_order"><img src="../img/icon_panier_no_cgi.png" width="100px"></a>
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
			Etat de votre panier 🧺
		</h1>
		<p class="cgi">
			avec CGI !
		</p>
        <div class="informations">
        <?php
			if (isset($_SESSION['cart']))
			{
				foreach ($_SESSION['cart'] as $cart)
				{
					echo '<br>Votre Nom : ', $cart['name'], '<br>';
					echo 'Votre Hamster : ', $cart['hamster'], '<br>';
					echo 'Sa Fourrure : ', $cart['color'], '<br>';
				}
			}
        ?>
		</div>
		<form action="../form.html">
			<input type="submit" value="Go acheter un nouvel ami !" class="bouton" />
		</form>
        <form action="./cgi_destroy.php">
			<input type="submit" value="Jeter votre panier et reformuler une adoption" class="bouton" />
		</form>
		<form action="../index.html">
			<input type="submit" value="Retour vers la page d'accueil" class="bouton" />
		</form>
	</body>
</html>
