let scene, camera, rendered, cube;

function parentWidth(elem) {
  return elem.parentElement.clientWidth;
}

function parentHeight(elem) {
  return elem.parentElement.clientHeight;
}

function init3D() {
  scene = new THREE.Scene();
  scene.background = new THREE.Color(0xffffff);

  camera = new THREE.PerspectiveCamera(
    75,
    parentWidth(document.getElementById("3Dcube")) /
      parentHeight(document.getElementById("3Dcube")),
    0.1,
    1000
  );

  renderer = new THREE.WebGLRenderer({ antialias: true });
  renderer.setSize(
    parentWidth(document.getElementById("3Dcube")),
    parentHeight(document.getElementById("3Dcube"))
  );

  document.getElementById("3Dcube").appendChild(renderer.domElement);

  // Create a geometry
  const geometry = new THREE.BoxGeometry(5, 1, 4);

  // Materials of each face
  var cubeMaterials = [
    new THREE.MeshBasicMaterial({ color: 0x200f21 }),
    new THREE.MeshBasicMaterial({ color: 0x382039 }),
    new THREE.MeshBasicMaterial({ color: 0x5a3d5c }),
    new THREE.MeshBasicMaterial({ color: 0x200f21 }),
    new THREE.MeshBasicMaterial({ color: 0x382039 }),
    new THREE.MeshBasicMaterial({ color: 0x5a3d5c }),
  ];

  const material = new THREE.MeshFaceMaterial(cubeMaterials);

  cube = new THREE.Mesh(geometry, material);
  scene.add(cube);
  camera.position.z = 5;
  renderer.render(scene, camera);
}

// Resize the 3D object when the browser window changes size
function onWindowResize() {
  camera.aspect =
    parentWidth(document.getElementById("3Dcube")) /
    parentHeight(document.getElementById("3Dcube"));
  //camera.aspect = window.innerWidth /  window.innerHeight;
  camera.updateProjectionMatrix();
  //renderer.setSize(window.innerWidth, window.innerHeight);
  renderer.setSize(
    parentWidth(document.getElementById("3Dcube")),
    parentHeight(document.getElementById("3Dcube"))
  );
}

window.addEventListener("resize", onWindowResize, false);

// Create the 3D representation
init3D();

// Create events for the sensor readings
if (!!window.EventSource) {
  var source = new EventSource("/events");

  source.addEventListener(
    "open",
    function (e) {
      console.log("Events Connected");
    },
    false
  );

  source.addEventListener(
    "error",
    function (e) {
      if (e.target.readyState != EventSource.OPEN) {
        console.log("Events Disconnected");
      }
    },
    false
  );

  source.addEventListener(
    "gyro_readings",
    function (e) {
      //console.log("gyro_readings", e.data);
      var obj = JSON.parse(e.data);
    
      // Change cube rotation after receiving the readinds
      cube.rotation.x = obj.gyroY;
      cube.rotation.z = obj.gyroX;
      cube.rotation.y = obj.gyroZ;
      renderer.render(scene, camera);
    },
    false
  );
}

function resetPosition(element) {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/" + element.id, true);
  console.log(element.id);
  xhr.send();
}
