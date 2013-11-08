using UnityEngine;
using System.Collections;

public class CameraController : MonoBehaviour {
	
	int speed = 50;
	
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		
		
		if (Input.GetKey("w"))
		{
			transform.Translate (0,0,speed*Time.deltaTime, Space.Self);
		}
		
		if (Input.GetKey("s"))
		{
			transform.Translate(0,0, -speed*Time.deltaTime, Space.Self);	
		}
		
	}
}
