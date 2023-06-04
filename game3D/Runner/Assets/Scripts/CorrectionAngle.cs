using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CorrectionAngle : MonoBehaviour
{
    public GameObject originalObject;

    private PlayerMovement pmScript;

    // Start is called before the first frame update
    void Start()
    {
        pmScript = FindObjectOfType<PlayerMovement>();
    }

    // Update is called once per frame
    void Update()
    {
        if (pmScript.jumpCount != 2)
        {
            transform.forward = originalObject.GetComponent<Transform>().forward;
        }
    }
}
