using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour
{
    [SerializeField] private GameObject mainMenuCanvas;
    [SerializeField] private GameObject gameCanvas;
    [SerializeField] private GameObject pauseCanvas;

    private bool gameStarted = false;

    // Start is called before the first frame update
    void Start()
    {
        Time.timeScale = 0f;
        mainMenuCanvas.SetActive(true);
        gameCanvas.SetActive(false);
        pauseCanvas.SetActive(false);
    }

    public void startGame()
    {
        Time.timeScale = 1f;
        gameStarted = true;

        mainMenuCanvas.SetActive(false);
        gameCanvas.SetActive(true);
    }

    public void pauseGame()
    {
        Time.timeScale = 0f;

        pauseCanvas.SetActive(true);
    }

    public void continueGame()
    {
        Time.timeScale = 1f;

        pauseCanvas.SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        if (!gameStarted && mainMenuCanvas.activeSelf && Input.GetKeyDown(KeyCode.Space))
        {
            startGame();
        }

        if (gameStarted && pauseCanvas.activeSelf && Input.GetKeyDown(KeyCode.Space))
        {
            continueGame();
        }
    }
}
