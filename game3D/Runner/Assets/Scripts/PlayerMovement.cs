using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static System.Collections.Specialized.BitVector32;

public class PlayerMovement : MonoBehaviour
{
    public GameObject level;
    // Start is called before the first frame update
    void Start()
    {
        // -2.75 9.5 -10
    }

    // Update is called once per frame
    void Update()
    {
        //GetComponent<Transform>.position
        if (Input.GetKeyDown("space"))
        {
            GetComponent<Rigidbody>().velocity = new Vector3(0, 5, 0);
        }
    }
}
