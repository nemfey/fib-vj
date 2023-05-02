using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMovement : MonoBehaviour
{
    public Vector3 velocity;
    float playerVelocity;

    // Start is called before the first frame update
    void Start()
    {
        playerVelocity = GameObject.Find("Player").GetComponent<PlayerMovement>().velocity;
        velocity = new Vector3(playerVelocity / 2, 0f, playerVelocity / 2);
    }

    // Update is called once per frame
    void Update()
    {
        playerVelocity = GameObject.Find("Player").GetComponent<PlayerMovement>().velocity;
        velocity = new Vector3(playerVelocity / 2, 0f, playerVelocity / 2);
        transform.Translate(velocity * Time.deltaTime);
    }
}
