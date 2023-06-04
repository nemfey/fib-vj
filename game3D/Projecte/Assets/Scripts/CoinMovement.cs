using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CoinMovement : MonoBehaviour
{
    public float rotationSpeed = 100f;
    public float floatSpeed = 2f;

    private Vector3 initialPosition;

    // Start is called before the first frame update
    void Start()
    {
        initialPosition = transform.position;
    }

    // Update is called once per frame
    void Update()
    {
        // Rotate
        transform.Rotate(0f, rotationSpeed * Time.deltaTime, 0f);

        // Float
        Vector3 newPosition = initialPosition;
        newPosition.y += Mathf.Sin(Time.time * floatSpeed) * 0.25f;
        transform.position = newPosition;
    }
}
