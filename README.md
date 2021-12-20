<h1> Time series & Nearest Neighbor </h1>

<h4>Ομάδα:</h4> Δημήτρης Χριστοφής 1115201800214 Κωνσταντίνος Θεοφίλης 1115201600287<br/>

<h3>Δομή αρχείων project:</h3><br/>
<p>Το project είναι υλοποιημένο και χωρισμένο σε modules. 
Ότι αφορά κώδικα βρίσκεται στον φάκελο /src, ο οποίος με την σειρά του χωρίζεται σε 5 υποφακέλους ανάλογα με τα ερωτήματα. Σημείωση: ο φάκελος /src/Common περιλαμβάνει κώδικα κοινό και για τις υλοποιήσεις που ζητούνται. Στο root του /src βρίσκονται δύο αρχεία main, ένα για την κάθε εκτέλεση. Τα προγραμματα μας μεταγλωττίζονται με χρήση Makefile. Στο μονοπάτι src/fred υπάρχει ο έτοιμος κώδικας που χρησιμοποιήθηκε για την υλοποίηση του ερωτήματος Aiii (Frechet Continuous).
<br /></p>
<p>Στον φάκελο dir υπάρχουν τα αρχεία εισόδου που ζητούνται απο την εκφώνηση (input file και query file για ερώτημα Α καθώς και cluster.conf για Β). Επίσης, στον ίδιο φάκελο /dir/output γίνεται και η αποθήκευση των output.<br /></p>
<p>Ότι αφορά το Unit Testing έχει δημιουργηθεί νέος φάκελος στο root του project /tests. Στο tests/lib υπάρχει η βιβλιοθήκη acutest.hpp η οποία αποτελεί το test framework που χρησιμοποιήσαμε. Στο test_unit.cpp έχουν ηλοποιηθεί ορισμένες βασικές test συναρτήσεις που με την σειρά τους ελέγχουν την λειτουργικότητα των συναρτήσεων μας.<br /></p>

Compile και Execution:

Question A: 
>Compile with: **make search** <br />
>Execution:
>>./search -i dir/nasd_input.csv -q dir/nasd_query.csv -o dir/output/outLSH.txt -algorithm LSH <br />
>>./search -i dir/nasd_input.csv -q dir/nasd_query.csv -o dir/output/outHypercube.txt -algorithm Hypercube <br />
>>./search -i dir/nasd_input.csv -q dir/nasd_query.csv -o dir/output/outFrechetDiscrete.txt -algorithm Frechet -metric discrete -delta 1.0<br />
>>./search -i dir/nasd_input.csv -q dir/nasd_query.csv -o dir/output/outFrechetContinuous.txt -algorithm Frechet -metric continuous -delta 1.0<br />

Question B: 
>Compile with: **make cluster**<br />
>Execution:
>>./cluster -i dir/nasd_input.csv -c dir/cluster.conf -o dir/output/outClusterVector.txt -update Mean_Vector -assignment Classic -complete -silhouette<br />
>>./cluster -i dir/nasd_input.csv -c dir/cluster.conf -o dir/output/outClusterFrechet.txt -update Mean_Frechet -assignment Classic -complete -silhouette<br />
