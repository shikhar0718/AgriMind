import db from "../config/firebase.js";

const getLatestSensorData = async () => {
  const snapshot = await db.ref("Agrimind").once("value");

  return snapshot.val();
};

export default {
  getLatestSensorData,
};