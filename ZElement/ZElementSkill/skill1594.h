#ifndef __CPPGEN_GNET_SKILL1594
#define __CPPGEN_GNET_SKILL1594
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1594:public Skill
    {
      public:
        enum
        { SKILL_ID = 1594 };
          Skill1594 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1594Stub:public SkillStub
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
      Skill1594Stub ():SkillStub (1594)
        {
            occupation = 150;
            name = L"Ä¦Ú­ÐÄ¾­II";
            nativename = "Ä¦Ú­ÐÄ¾­II";
            icon = "Ä¦Ú­ÐÄ¾­II.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 11;
            clearmask = 132;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 3;
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
            action[0] = "ÌìÒôËÂ_Ä¦Ú­ÐÄ¾­";
            action[1] = "ÌìÒôËÂ_Ä¦Ú­ÐÄ¾­";
            action[2] = "ÌìÒôËÂ_Ä¦Ú­ÐÄ¾­";
            action[3] = "ÌìÒôËÂ_Ä¦Ú­ÐÄ¾­";
            action[4] = "ÌìÒôËÂ_Ä¦Ú­ÐÄ¾­";
            action[5] = "ÌìÒôËÂ_Ä¦Ú­ÐÄ¾­";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_Ä¦Ú­ÐÄ¾­";
            action[16] = "ÌìÒôËÂ_Æï³Ë_Ä¦Ú­ÐÄ¾­";
            action[17] = "ÌìÒôËÂ_Æï³Ë_Ä¦Ú­ÐÄ¾­";
            action[18] = "ÌìÒôËÂ_Æï³Ë_Ä¦Ú­ÐÄ¾­";
            action[19] = "ÌìÒôËÂ_Æï³Ë_Ä¦Ú­ÐÄ¾­";
            action[20] = "ÌìÒôËÂ_Æï³Ë_Ä¦Ú­ÐÄ¾­";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 2;
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
        virtual ~ Skill1594Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 549 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 8 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (30);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (2750 + 275 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 300000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (20);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               2750 + 275 * skill->GetLevel (),
                               15,
                               2400 * skill->GetLevel () * skill->GetLevel () + 4800, (2750 + 275 * skill->GetLevel ()) * 4, 3 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (3000 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetTurndebuff (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (4 * (2750 + 275 * skill->GetLevel ()) +
                                           (4800 * skill->GetLevel () * skill->GetLevel () +
                                            9600) * 0.02 * norm (skill->GetPlayer ()->GetLevel () / 159.5));
            skill->GetVictim ()->SetHpgen (1);
            skill->GetVictim ()->SetProbability (1.0 * (100 * norm (159.5 / skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount ((4800 * skill->GetLevel () * skill->GetLevel () + 9600) *
                                            norm (159.5 / skill->GetPlayer ()->GetLevel ()));
            skill->GetVictim ()->SetExpboost (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20);
        }
#endif
    };
}
#endif
