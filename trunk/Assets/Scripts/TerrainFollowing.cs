using UnityEngine;
using System.Collections;

public class TerrainFollowing : MonoBehaviour {
	
	int offset = 40;
	
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
		transform.position = new Vector3(transform.localPosition.x,50+Terrain.activeTerrain.SampleHeight(transform.position),transform.localPosition.z);
	}
}
