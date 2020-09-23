# persistanceErdos
Calcul de la persistance à la Erdos en PHP sur des grands nombres N de la forme 8888888, 7777777777, etc.<br/>
Pour en savoir plus sur la persistance à la Erdos d'un nombre : http://villemin.gerard.free.fr/aNombre/MOTIF/Chiffres/Multant.htm#erdos<br/>
usage : php gmp_persistance.php &lt;digit&gt; &lt;length&gt;<br/>
ex : php gmp_persistance.php 8 14 calule la persistance de 88888888888888, puis des 8...8 avec plus de 14 chiffres.<br/>
Les meilleures persistances sont affichées et stockées dans un fichier.<br/>
Ex de temps de calcul :<br/>
<pre>&gt;php gmp_persistance.php 8 1489223<br/>
p(8^1489223)=25 1'296
</pre>
Merci à Julien Courties pour ses échanges et son idée d'optimisation en regoupant les chiffres de N.<br/>
<br/>
<i>23/09/2020</i> : Ajout d'une version en C++ pour Linux, qui est 4x plus rapide :)
<pre>&gt;pErdos 8 1489223<br/>
p(8^1489223)=25 (in 0.425 s)
</pre>
