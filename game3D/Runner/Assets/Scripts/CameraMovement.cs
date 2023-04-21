using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMovement : MonoBehaviour
{
    public Vector3 speed;
    
    // Start is called before the first frame update
    void Start()
    {
        speed = new Vector3(2f, 0f, 2f);
    }

    // Update is called once per frame
    void Update()
    {
        transform.Translate(speed * Time.deltaTime);
    }
}
