import admin from "firebase-admin";
import serviceAccount from "../../serviceAccountKey.json" with { type: "json" };

admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL: "https://agrimind-5c81e-default-rtdb.firebaseio.com"
});

const db = admin.database();

export default db;