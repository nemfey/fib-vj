using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    [SerializeField] private GameObject mainMenuCanvas;
    [SerializeField] private GameObject gameCanvas;
    [SerializeField] private GameObject pauseCanvas;

    private static bool gameStarted = false;

    //private static bool  = false;

    // Start is called before the first frame update
    void Start()
    {
        if (!gameStarted)
        {
            Time.timeScale = 0f;
            //gameStarted = false;
            mainMenuCanvas.SetActive(true);
            gameCanvas.SetActive(false);
            pauseCanvas.SetActive(false);
        }
        else
        {
            startGame();
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (!gameStarted && mainMenuCanvas.activeSelf && Input.GetKeyDown(KeyCode.Space))
        {
            startGame();
        }
        
        if (gameStarted && !pauseCanvas.activeSelf && (Input.GetKeyDown(KeyCode.Escape) || Input.GetKeyDown(KeyCode.P)))
        {
            pauseGame();
        }
        else if (gameStarted && pauseCanvas.activeSelf && (Input.GetKeyDown(KeyCode.Escape) || Input.GetKeyDown(KeyCode.P)))
        {
            resumeGame();
        }

    }

    public void startGame()
    {
        Time.timeScale = 1f;
        gameStarted = true;
        FindObjectOfType<AudioManager>().playSound("MainSong");

        mainMenuCanvas.SetActive(false);
        gameCanvas.SetActive(true);
    }

    public void pauseGame()
    {
        Time.timeScale = 0f;

        // Hide pause button
        Transform pauseButtonTransform = gameCanvas.transform.Find("PauseButton");
        SetActiveButton(pauseButtonTransform, false);

        pauseCanvas.SetActive(true);
    }

    public void resumeGame()
    {
        Time.timeScale = 1f;

        // Show pause button
        Transform pauseButtonTransform = gameCanvas.transform.Find("PauseButton");
        SetActiveButton(pauseButtonTransform, true);
        
        pauseCanvas.SetActive(false);
    }

    public void restartGame()
    {
        SceneManager.LoadScene("Game");
    }

    public void showOptionsPause()
    {
        //
    }

    public void quitGame()
    {
        gameStarted = false;
        SceneManager.LoadScene("Game");
    }

    void SetActiveButton(Transform buttonTransform, bool b)
    {
        Button button = buttonTransform.GetComponent<Button>();

        if (button != null)
        {
            button.gameObject.SetActive(b);
        }
    }
}
