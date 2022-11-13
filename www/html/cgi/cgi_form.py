#! /usr/bin/python

import cgi

# permet de recevoir la request de l'user
form = cgi.FieldStorage()

# on cherche le contenu de chaque key du form
if form.has_key("name"):
	nom = form["name"].value
else :
	nom = "Champ 'name' vide !"

if form.has_key("hamster"):
	hamster = form["hamster"].value
else :
	hamster = "Champ 'hamster' vide !"

if form.has_key("color"):
	couleur = form["color"].value
else :
	couleur = "Champ 'color' vide !"

# test
print("Name : {nom}\nHamster : {hamster}\nColor : {couleur}\n")
