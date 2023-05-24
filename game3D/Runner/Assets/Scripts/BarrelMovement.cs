using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BarrelMovement : MonoBehaviour
{
    public float movementSpeed = 15f;

    // Update is called once per frame
    void Update()
    {
        // Move
        transform.position += -transform.right * movementSpeed * Time.deltaTime;
    }
}
