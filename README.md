# indoorlocation

To locate objects or persons inside buildings (out of GPS range) one option is to use beaconing systems. This is the aim of this project.

It should be possible to estimate the presence of a person between beacon points. When in range of the beacons the location should be more precise, when out of range of all beacons and estimation based on last movement and on accelerometer and gyroscope input should be used.

The indication of the position should be done on a building plan (blueprint). It should be indicated the arrival at a beacon, which is identified by an ID/code.

It should be possible to trace a route of the person even on places it has no connectivity and/or beacon coverage.

Requirements: support more than one person tracking estimation should accurate at least at corridor level (in what corridor) detection of "at a beacon" should be detected to at least 1 m proximity and never to more than 2.5 m. notification of arrival should be in less than 2s

