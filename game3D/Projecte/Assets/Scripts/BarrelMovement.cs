using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BarrelMovement : MonoBehaviour
{
    public float movementSpeed = 2f;

    // Update is called once per frame
    void Update()
    {
        // Move
        transform.position += -transform.right * movementSpeed * Time.deltaTime;
    }
}
