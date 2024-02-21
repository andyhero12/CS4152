Andrew Cheng
akc55
Developed on Xcode

1. Create PhotonSet and Photon
Copied AsteroidSet and Asteroid as reference for building class

2. Generate Photon objects
Copied spawnAsteroid with a new method spawnPhoton. Creates the photon with Vec2

3. Draw the Photons
Copied draw method from AsteroidSet and Asteroid to display wrap around, as well as
radius alignment for different scales

4. Add a Sound Effect
Copied Ship-Asteroid collision initialization, play, and destruction. Used asset file to retrieve audio. 

5. Add Collision Code
Copied Code for Ship-Asteroid collision for Photon-Asteroid collisions. Iterated through each photon and asteroid to check for collisions and removed elements based on iterators. 

6. End the Game.
Created a boolean variable to determine if game has ended. Terminated update method early when game is over. Scaled text using affine. Properly made reset code. 