O sa incep prin a explica fiecare fisier

In buffer se afla codul din laborator pt formatarea stringurilor atunci cand trimit requesturile

In connection e tot codul din laborator pt a putea face legatura cu serverul si a putea comunica cu el.

In parson se afla biblioteca pt json in C, atunci cand trebuie sa trimit niste fielduri in requesturi.

In request e aproximativ la fel codul din laborator, numai ca am facut requesturile specifice cerintei:
- In cazul de post o sa trimitem pe o linie url, pe a doua hostul, pe urmatoarele doua  cookieul si tokenul in cazul in care sunt valabile si payload-ul specific taskului.
- In cazul de delete o sa avem doar ruta de acces, hostul, tokenul si cookieul.
- In cazul de get avem la fel ca la Delete numai ca avem metoda GET.

In helper avem functiile pe care o sa le apelam in clientul meu numit program:
- Avem 3 functii de extract in care ne scoate cookieul, tokenul si fieldurile cartiilor primite de la server. Efectiv ma folosesc de strstr pt a vedea unde se afla fiecare, iar in cazul fieldurilor tb sa inceapa cu { , [ si se termina cu }, ].
- Functia create_json_user_object: folosindu-ne de biblioteca parson o sa ne geneream un obiect de tip JSON pt a putea prelucra usernameul si parola care urmeaza a fi citite pentru register si login. De ce parea citirea mai dubioasa? Ca nu cumva sa mi se suprapuna stringurile sau sa intampin probleme, de asemenea se da un clear la buffer de fiecare data cand e citit. Adica citirea e facuta astfel incat sa se opreasca la endline nu dupa spatiu, ma rog in cazul asta o sa afisam eroare daca intampinam spatiu si getchar e folosit pt a elimina orice caracter a ramas in bufferul de input.

- Functia register_user: mai intai verifica daca nu cumva userul este invalid si atunci se afiseaza mesajul de eroare. In cazul in care totul este in regula o sa face un post_request asa cum e sugerat in API-ul temei. Dupa care o sa verificam codul intors. Daca codul de intoarcere este 201 inseamna ca a fost creat cu succes, daca nu inseamna ca deja exista.

- Functia login: o sa trimita tot un post request numai ca o sa intoarca un cookie pentru a ne asigura ca suntem logati. Mi-am mai luat un flag pt a verifica in enter_library si alte comenzi daca avem voie sa facem requesturile corespunzatoare. Daca totul e ok flagul e setat pe 1 si cookieul este luat din server.

- Functia access_library: mai intai vedem daca suntem logati, daca nu avem de a face cu o eroare, daca suntem atunci vrem sa luam tokenul de la server ce ne asigura faptul ca avem acces in biblioteca si putem faca orice actiune care depinde de biblioteca. O sa setam si flagul de acces pe 1 pentru a ne asigura ca suntem privilegiati.

Obs: Nu are rost sa mai spun ce request trimit pentru ca sunt specificate in indrumarul temei.

- Functia get_books: In functia aceasta o sa vrem sa luam de la server id-urile si cartile inregistrate pana acum. 

- Functia get_book_details: E asemanatoare cu cea de mai sus numai ca o sa primim toate caracteristicile cartii specifice dupa un id. De asemenea verificam validitatea inputului ca sa nu avem surprize.

- Functia add_book_helper: Aici verificam daca inputurile sunt ok din punct de vedere al validitatii si daca sunt atunci le adaugam in obiectul nostru JSON pt a putea fi trimise catre server. Am explicat mai sus citirea stringurilor.

- Functia add_book: Aici vedem daca putem trimite requestul sau nu si daca il trimitem verificam si codul de intoarcere.

- Functia delete_book_from_library: Actioneaza la fel ca functiile de mai sus numai ca dam un delete_request si verificam codul, la fel si pt functia de logout numai ca facem un get_request.

Program.c
In program.c facem conexiunea, setam variabilele descrise mai sus si apelam functiile povestite.