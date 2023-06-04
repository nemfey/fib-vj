using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BoatMovement : MonoBehaviour
{
    private GameManager gameManagerScript;

    public float velocity = 10f;

    // Start is called before the first frame update
    void Start()
    {
        GameObject gameManagerObject = GameObject.Find("GameManager");
        gameManagerScript = gameManagerObject.GetComponent<GameManager>();

        velocity = 10f;
    }

    // Update is called once per frame
    void Update()
    {
        if (gameManagerScript.cutSceneStarted && Time.timeScale == 1f)
        {
            transform.position += new Vector3(0.25f, 0.5f, 0.25f);
        }
    }
}
