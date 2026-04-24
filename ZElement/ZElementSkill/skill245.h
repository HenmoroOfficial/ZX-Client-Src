#ifndef __CPPGEN_GNET_SKILL245
#define __CPPGEN_GNET_SKILL245
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill245:public Skill
    {
      public:
        enum
        { SKILL_ID = 245 };
          Skill245 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill245Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill245Stub ():SkillStub (245)
        {
            occupation = 1;
            name = L"Àä³°ÈÈ·í";
            nativename = "Àä³°ÈÈ·í";
            icon = "Àä³°ÈÈ·í.dds";
            maxlevel = 7;
            maxlearn = 6;
            type = 3;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 0;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 1;
            talent[0] = 639;
            talent[1] = 645;
            talent_size = 2;
            action[0] = "¹íÍõ×Ú_¼¤Å­";
            action[1] = "¹íÍõ×Ú_¼¤Å­";
            action[2] = "¹íÍõ×Ú_¼¤Å­";
            action[3] = "¹íÍõ×Ú_¼¤Å­";
            action[4] = "¹íÍõ×Ú_¼¤Å­";
            action[5] = "¹íÍõ×Ú_¼¤Å­";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_¼¤Å­";
            action[16] = "¹íÍõ×Ú_Æï³Ë_¼¤Å­";
            action[17] = "¹íÍõ×Ú_Æï³Ë_¼¤Å­";
            action[18] = "¹íÍõ×Ú_Æï³Ë_¼¤Å­";
            action[19] = "¹íÍõ×Ú_Æï³Ë_¼¤Å­";
            action[20] = "¹íÍõ×Ú_Æï³Ë_¼¤Å­";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷1";
            rangetype = 0;
            doenchant = true;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill245Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 243 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12 + 2 * skill->GetLevel ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.4 * skill->GetLevel () * skill->GetLevel () + 6.5 * skill->GetLevel () + 62);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 10000 * skill->GetLevel () - 10000 * skill->GetT0 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[6] = { 25, 30, 35, 41, 43, 47 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (10 + 2 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (8 + 2 * skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (0.4 * skill->GetLevel () * skill->GetLevel () + 6.5 * skill->GetLevel () + 62),
                               65 + 5 * skill->GetLevel (), 120 - 10 * skill->GetLevel (), 8 + 2 * skill->GetLevel (), 30 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (65 + 5 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (12000 + 100);
            skill->GetVictim ()->SetAssault (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (2000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->
                SetAmount ((0.15 + 0.15 * skill->GetLevel () + 0.01 * skill->GetLevel () * skill->GetLevel () +
                            0.06 * skill->GetT0 () * skill->GetLevel ()) * (INT (zrand (100) / (100 - 6 * skill->GetT0 ())) + 1));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak5 (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (2050 * skill->GetT0 ());
            skill->GetVictim ()->SetImmunesilent (1);
            return true;
        }
#endif
    };
}
#endif
