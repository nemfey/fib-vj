using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static System.Collections.Specialized.BitVector32;

public class PlayerMovement : MonoBehaviour
{
    public GameObject level;
    public uint remainingJumps;
    // Start is called before the first frame update
    void Start()
    {
        remainingJumps = 2;
    }

    // Update is called once per frame
    void Update()
    {
        if (GetComponent<Rigidbody>().velocity.y == 0 && remainingJumps < 2)
            remainingJumps = 2;

        if (Input.GetKeyDown("space") && remainingJumps > 0)
        {
            GetComponent<Rigidbody>().velocity = new Vector3(0, 5, 0);
            --remainingJumps;
        }

        //level.newSectionProcedure();
    }
}
