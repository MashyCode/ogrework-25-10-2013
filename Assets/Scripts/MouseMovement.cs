using UnityEngine;
using System.Collections;

public class MouseMovement : MonoBehaviour {
	
	public float rotationSpeed = 50;
	
	
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		float yaw = Input.GetAxis("Mouse X") * rotationSpeed;
		float pitch = Input.GetAxis ("Mouse Y") * -rotationSpeed;
		pitch *= Time.deltaTime;
		yaw *= Time.deltaTime;
		transform.Rotate (pitch, yaw, 0);
	}
}
