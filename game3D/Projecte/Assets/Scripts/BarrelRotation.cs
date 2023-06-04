using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BarrelRotation : MonoBehaviour
{
    public float rotationSpeed = 100f;

    // Update is called once per frame
    void Update()
    {
        // Rotate
        transform.Rotate(0f, 0f, rotationSpeed * Time.deltaTime);
    }
}
